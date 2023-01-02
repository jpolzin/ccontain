import subprocess
import typing
import os
import pdb

file_path = os.path.dirname(os.path.realpath(__file__))
initial_path = os.getcwd()
build_path = f'{file_path}/build'

def run_command(command: str, **kwargs):
    print(command)
    subprocess.run(command.split(' '))


try:
    os.mkdir(f'{build_path}')
except FileExistsError:
    pass
os.chdir(f'{build_path}')
run_command(f'cd {build_path}')
run_command(f'cmake -DCMAKE_BUILD_TYPE=Debug ..')
run_command(f'cmake --build .')
os.chdir(f'{initial_path}')
