#!/usr/bin/env python3
"""Check source/test layout, catalog coverage, Git blob IDs, and Markdown links.

Standard library only. This is a documentation consistency check, not a C++
parser or correctness test. No files are modified.
"""

import hashlib
import re
import sys
from pathlib import Path
from urllib.parse import unquote, urlsplit


SOURCE_DIR = Path('library')


def library_sources(root):
    """Keep coverage and compilation on the same set of library files."""
    return {
        p.relative_to(root).as_posix(): p
        for p in (root / SOURCE_DIR).rglob('*.cpp')
    }


def check_layout(root, sources):
    """Check matching paths/includes without fixing the set of categories."""
    errors = []
    if not (root / SOURCE_DIR).is_dir():
        errors.append('Missing library/ source directory')
    excluded = {'tests', 'test', 'tools', 'build', '.git', 'templates', 'snippets'}
    for path in sorted(root.rglob('*.cpp')):
        relative = path.relative_to(root)
        if any(part in excluded for part in relative.parts[:-1]):
            continue
        if relative.parts[0] != SOURCE_DIR.name:
            errors.append(f'Library must be under library/<category>/: {relative.as_posix()}')
    for name in sorted(sources):
        relative = Path(name)
        if len(relative.parts) < 3 or relative.parts[0] != SOURCE_DIR.name:
            errors.append(f'Library must be under library/<category>/: {name}')
            continue
        test = root / 'tests' / relative.relative_to(SOURCE_DIR).with_suffix('.test.cpp')
        if not test.is_file():
            errors.append(f'Missing matching regression test: {test.relative_to(root)}')
            continue
        includes = re.findall(r'^\s*#\s*include\s*"([^"]+)"',
                              test.read_text(encoding='utf-8'), flags=re.MULTILINE)
        if name not in includes:
            errors.append(f'Test must include {name}: {test.relative_to(root)}')
    for test in sorted((root / 'tests').rglob('*.test.cpp')):
        relative = test.relative_to(root / 'tests').as_posix()
        name = (SOURCE_DIR / (relative[:-len('.test.cpp')] + '.cpp')).as_posix()
        if name not in sources:
            errors.append(f'Regression test has no matching library: {test.relative_to(root)}')
    return errors


def check(root):
    catalog = root / 'Rule' / 'NamingAndAPI.md'
    errors = []
    if not catalog.is_file():
        return ['Missing Rule/NamingAndAPI.md']

    text = catalog.read_text(encoding='utf-8')
    declarations = re.findall(
        r'^<!-- catalog-source: ([^\s]+\.cpp) ([0-9a-f]{40}) -->$',
        text,
        flags=re.MULTILINE,
    )
    recorded = {}
    for name, sha in declarations:
        if name in recorded:
            errors.append(f'Duplicate catalog source: {name}')
        recorded[name] = sha

    # Discover all categories under library/; keep management files separate.
    actual = library_sources(root)
    errors.extend(check_layout(root, actual))
    for name in sorted(actual.keys() - recorded.keys()):
        errors.append(f'Library missing from catalog: {name}')
    for name in sorted(recorded.keys() - actual.keys()):
        errors.append(f'Catalog source missing from repository: {name}')
    for name in sorted(actual.keys() & recorded.keys()):
        data = actual[name].read_bytes()
        sha = hashlib.sha1(f'blob {len(data)}\0'.encode('ascii') + data).hexdigest()
        if sha != recorded[name]:
            errors.append(f'Source changed; review catalog: {name} (current blob {sha})')

    markdown = [root / 'README.md', *sorted((root / 'Rule').rglob('*.md'))]
    for directory in ('library', 'tests', 'tools', 'records'):
        markdown.extend(sorted((root / directory).rglob('*.md')))
    if (root / 'AGENTS.md').is_file():
        markdown.append(root / 'AGENTS.md')
    for path in markdown:
        if not path.is_file():
            errors.append(f'Missing Markdown file: {path.relative_to(root)}')
            continue
        # Ignore fenced examples. Actual links in these files use inline form.
        body = re.sub(r'```.*?```', '', path.read_text(encoding='utf-8'), flags=re.DOTALL)
        for target in re.findall(r'\[[^\]\n]*\]\(([^\s)]+)\)', body):
            parts = urlsplit(target)
            if parts.scheme or parts.netloc or not parts.path:
                continue
            destination = (path.parent / unquote(parts.path)).resolve()
            if not destination.exists():
                errors.append(f'Broken relative link in {path.relative_to(root)}: {target}')

    print(f'Libraries: {len(actual)}; catalog entries: {len(recorded)}; Markdown files: {len(markdown)}')
    return errors


def main():
    root = Path(__file__).resolve().parents[1]
    errors = check(root)
    if errors:
        for error in errors:
            print(f'ERROR: {error}', file=sys.stderr)
        return 1
    print('OK: source/test layout, coverage, source fingerprints, and relative links agree.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
