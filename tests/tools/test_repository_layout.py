"""Guard category moves and automatic discovery of new source directories."""

import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from check_catalog import check_layout, library_sources


def save(root, name, text):
    path = root / name
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding='utf-8')


class RepositoryLayoutChecks(unittest.TestCase):
    def test_tree_and_new_category_are_discovered(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in ('tree/Example.cpp', 'new_category/Another.cpp'):
                save(root, name, '// library\n')
                test = 'tests/' + name.removesuffix('.cpp') + '.test.cpp'
                save(root, test, f'#include "{name}"\nint main() {{}}\n')
            save(root, 'templates/AtCoder.cpp', 'int main() {}\n')
            sources = library_sources(root)
            self.assertEqual(set(sources), {'tree/Example.cpp', 'new_category/Another.cpp'})
            self.assertEqual(check_layout(root, sources), [])

    def test_moving_only_the_library_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            save(root, 'tree/Example.cpp', '// moved library\n')
            save(root, 'tests/graph/Example.test.cpp', '#include "graph/Example.cpp"\n')
            errors = check_layout(root, library_sources(root))
            self.assertTrue(any('Missing matching regression test' in error for error in errors))
            self.assertTrue(any('no matching library' in error for error in errors))

    def test_stale_include_after_moving_both_files_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            save(root, 'tree/Example.cpp', '// moved library\n')
            save(root, 'tests/tree/Example.test.cpp', '#include "graph/Example.cpp"\n')
            errors = check_layout(root, library_sources(root))
            self.assertTrue(any('Test must include tree/Example.cpp' in error for error in errors))

    def test_root_and_management_sources_are_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in ('Example.cpp', 'Rule/Another.cpp'):
                save(root, name, '// misplaced library\n')
                test = 'tests/' + name.removesuffix('.cpp') + '.test.cpp'
                save(root, test, f'#include "{name}"\n')
            errors = check_layout(root, library_sources(root))
            self.assertEqual(sum('source category' in error for error in errors), 2)


if __name__ == '__main__':
    unittest.main()
