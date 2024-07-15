import os

def get_header_files(directory):
    header_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.h'):
                # 获取相对于src目录的路径
                rel_path = os.path.relpath(os.path.join(root, file), directory)
                header_files.append(f'#include "{rel_path.replace(os.sep, "/")}"')
    return header_files

def write_to_file(header_files, output_file):
    with open(output_file, 'w') as f:
        for header in header_files:
            f.write(header + '\n')

if __name__ == "__main__":
    src_directory = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    output_file = os.path.join(src_directory, 'test/header.inc')
    
    header_files = get_header_files(src_directory)
    write_to_file(header_files, output_file)
