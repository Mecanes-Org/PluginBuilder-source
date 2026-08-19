import sys
import zipfile
import os

def zip_directory(directory, output_zip):
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zf:
        for root, dirs, files in os.walk(directory):
            for file in files:
                file_path = os.path.join(root, file)
                arcname = os.path.relpath(file_path, directory)
                zf.write(file_path, arcname)

def zip_single_file(file_path, output_zip):
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zf:
        zf.write(file_path, os.path.basename(file_path))

if __name__ == "__main__":
    if len(sys.argv) < 3:
        # print("Usage: zipper.exe <source> <output.zip>")
        sys.exit(1)

    source = sys.argv[1]
    output = sys.argv[2]
    
    # FIX : protection contre les valeurs None (bug PyInstaller)
    if source is None or not isinstance(source, str):
        sys.exit(1)
    if output is None or not isinstance(output, str):
        sys.exit(1)

    source = source.strip()
    output = output.strip()

    if not source:
        # print("Source est None")
        sys.exit(1)
    if not output:
        # print("Output est None")
        sys.exit(1)

    try:
        if os.path.isdir(source):
            zip_directory(source, output)
        else:
            zip_single_file(source, output)
        # print(f"Archive créée: {output}")
        sys.exit(0)
    except Exception as e:
        # print(f"Erreur: {e}")
        sys.exit(1)
