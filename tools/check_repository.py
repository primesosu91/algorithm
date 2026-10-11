#!/usr/bin/env python3
"""Read-only C++23 checks, shared by local execution and GitHub Actions.

Checks documentation, parses each library independently, and builds/runs saved
regression programs. It neither submits to a judge nor changes adoption status.
"""

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from check_catalog import check, library_sources
from generate_snippets import TEMPLATE, build_snippets, check_snippets


REQUIRED = (
    'AGENTS.md',
    'Rule/LibraryDevelopmentRules.md',
    'Rule/CompetitiveProgrammingGuide.md',
    'Rule/DebugRules.md',
    'Rule/LibraryWorkflow.md',
    'Rule/README.md',
    'Rule/SnippetRules.md',
    'Rule/ProjectInstructions.md',
    'records/LibraryStatus.md',
    'records/DevelopmentLog.md',
    'templates/AtCoder.cpp',
    'snippets/competitive_programming.code-snippets',
)


def run(command, root, timeout):
    try:
        result = subprocess.run(command, cwd=root, capture_output=True,
                                text=True, timeout=timeout, check=False)
    except (OSError, subprocess.TimeoutExpired) as error:
        print(f'ERROR: {error}', file=sys.stderr)
        return False
    if result.stdout:
        print(result.stdout, end='')
    if result.stderr:
        print(result.stderr, end='', file=sys.stderr)
    if result.returncode:
        print(f'ERROR: exit {result.returncode}: {command[0]}', file=sys.stderr)
    return result.returncode == 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path,
                        default=Path(__file__).resolve().parents[1])
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--sanitizers', action='store_true')
    parser.add_argument('--skip-syntax', action='store_true',
                        help='Reuse syntax checks already passed for this tree')
    args = parser.parse_args()
    root = args.root.resolve()

    errors = [f'Missing {path}' for path in REQUIRED
              if not (root / path).is_file()]
    errors.extend(check(root))
    errors.extend(check_snippets(root))
    if errors:
        for error in errors:
            print(f'ERROR: {error}', file=sys.stderr)
        return 1
    compiler = shutil.which(args.cxx)
    if not compiler:
        print(f'ERROR: C++ compiler not found: {args.cxx}', file=sys.stderr)
        return 1

    libraries = sorted(library_sources(root).values())
    tests = sorted((root / 'tests').rglob('*.test.cpp'))
    if not libraries or not tests:
        print('ERROR: no library sources or no saved regression tests',
              file=sys.stderr)
        return 1

    flags = ['-std=c++23', '-Wall', '-Wextra', '-I', str(root)]
    print('TEST snippet generation and expansion', flush=True)
    if not run([sys.executable, '-m', 'unittest', 'discover', '-s', 'tests/tools',
                '-p', 'test_*.py'], root, 30):
        return 1
    if not args.skip_syntax:
        for source in libraries:
            print(f'CHECK {source.relative_to(root)}', flush=True)
            if not run([compiler, *flags, '-fsyntax-only', str(source)],
                       root, 120):
                return 1
        template = (root / TEMPLATE).read_text(encoding='utf-8')
        split = template.index('void solve() {')
        with tempfile.TemporaryDirectory(prefix='algorithm-snippet-') as directory:
            source = Path(directory) / 'expanded.cpp'
            source.write_text(template, encoding='utf-8')
            if not run([compiler, *flags, '-fsyntax-only', str(source)], root, 120):
                return 1
            for library in libraries:
                print(f'SNIPPET {library.stem}', flush=True)
                body = library.read_text(encoding='utf-8').split('\n', 2)[2]
                source.write_text(template[:split] + body + '\n' + template[split:],
                                  encoding='utf-8')
                if not run([compiler, *flags, '-fsyntax-only', str(source)], root, 120):
                    return 1
            print('SNIPPET all libraries together', flush=True)
            bodies = '\n'.join(library.read_text(encoding='utf-8').split('\n', 2)[2]
                               for library in libraries)
            source.write_text(template[:split] + bodies + '\n' + template[split:],
                              encoding='utf-8')
            if not run([compiler, *flags, '-fsyntax-only', str(source)], root, 120):
                return 1

    test_flags = ['-O2', '-D_GLIBCXX_ASSERTIONS']
    if args.sanitizers:
        # Non-PIE avoids ASan shadow-memory collisions in some container hosts.
        test_flags = ['-O1', '-g', '-D_GLIBCXX_ASSERTIONS',
                      '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                      '-fno-omit-frame-pointer', '-fno-pie', '-no-pie']
    with tempfile.TemporaryDirectory(prefix='algorithm-check-') as directory:
        for index, source in enumerate(tests):
            executable = str(Path(directory) / f'test-{index}')
            print(f'TEST {source.relative_to(root)}', flush=True)
            if not run([compiler, *flags, *test_flags, str(source), '-o', executable],
                       root, 120):
                return 1
            if not run([executable], root, 30):
                return 1

    mode = 'ASan/UBSan' if args.sanitizers else 'standard'
    syntax = 'reused' if args.skip_syntax else str(len(libraries))
    print(f'OK: documentation and {len(build_snippets(root))} snippets; '
          f'C++23 syntax {syntax}; {len(tests)} regression programs ({mode}). AC is not checked.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
