import os
import sys

def patch_nfsim():
    src_root = "bng2/nfsim_src"

    # Files and their patches
    patches = {
        "src/NFinput/NFinput.cpp": [
            ("2566s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2594s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2606s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2627s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2637s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2669s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2680s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2696s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2750s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2802s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2813s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2837s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2923s/return false;/return NULL;/", "return false;", "return NULL;"),
            ("2944s/return false;/return NULL;/", "return false;", "return NULL;"),
        ],
        "src/NFcore/molecule.cpp": [
            ("346s/==NOBOND/ == NULL/", "==NOBOND", " == NULL"),
            ("371s/!=NOBOND/ != NULL/", "!=NOBOND", " != NULL"),
            ("398s/==NOBOND/ == NULL/", "==NOBOND", " == NULL"),
            ("404s/==NOBOND/ == NULL/", "==NOBOND", " == NULL"),
            ("432s/!=NOBOND/ != NULL/g", "!=NOBOND", " != NULL"),
        ],
        "src/NFcore/templateMolecule.cpp": [
            ("1177s/==Molecule::NOBOND/ == NULL/", "==Molecule::NOBOND", " == NULL"),
            ("1282s/==Molecule::NOBOND/ == NULL/", "==Molecule::NOBOND", " == NULL"),
        ]
    }

    for rel_path, file_patches in patches.items():
        abs_path = os.path.join(src_root, rel_path)
        if not os.path.exists(abs_path):
            print(f"Warning: {abs_path} not found")
            continue

        with open(abs_path, 'r') as f:
            lines = f.readlines()

        modified = False
        for _, search, replace in file_patches:
            for i, line in enumerate(lines):
                if search in line:
                    lines[i] = line.replace(search, replace)
                    modified = True

        if modified:
            with open(abs_path, 'w') as f:
                f.writelines(lines)
            print(f"Patched {abs_path}")

if __name__ == "__main__":
    patch_nfsim()
