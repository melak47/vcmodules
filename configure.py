﻿import pathlib
from pathlib import Path
import ninja_syntax
from ninja_syntax import escape_path

#region boilerplate

Path = pathlib.Path
def path_str(self):
    return str(pathlib.PurePosixPath(self))
Path.__str__ = path_str

cwd = Path('.').resolve()
ninja = Path('build.ninja').open(mode='w', encoding='utf-8')

def scan_imports(src: Path, key: Path):
    dyndep = key.with_suffix('.dd')
    ninja.write(f'build {dyndep}: import_scan {src}\n')
    ninja.write(f'    key = {key}\n')
    ninja.write('\n')
    return dyndep

def compile(src: Path):
    obj = Path('build') / (src.name + '.obj')
    dyndep = scan_imports(src=src, key=obj)
    pdb = obj.with_suffix('.pdb')

    ninja.write(f'build {obj} | {pdb}: compile {src} || {dyndep}\n')
    ninja.write(f'    obj = {obj}\n')
    ninja.write(f'    pdb = {pdb}\n')
    ninja.write(f'    dyndep = {dyndep}\n')
    ninja.write('\n')

    return obj

def interface(src, lib_prefix=''):
    obj = Path('build') / src.parent / (src.name + '.obj')
    pdb = obj.with_suffix('.pdb')
    ifc = Path('build') / 'ifc' / Path(lib_prefix + src.stem + '.ifc')
    dyndep = scan_imports(src=src, key=ifc)

    ninja.write(f'build {ifc} | {obj} {pdb}: module_interface {src} || {dyndep}\n')
    ninja.write(f'    obj = {obj}\n')
    ninja.write(f'    pdb = {pdb}\n')
    ninja.write(f'    dyndep = {dyndep}\n')
    ninja.write('\n')

    return obj

def sources(directory: Path):
    src = []
    for ext in ['cc','cpp','cxx','c++']:
        src.extend([x for x in directory.glob(f'**/*.{ext}')])
    return src

def modules(directory: Path):
    return [x for x in directory.glob('**/*.ixx')]

def compile_directory(directory, lib_prefix=''):
    objects = []
    for src in sources(directory):
        objects.append(compile(src))
    for ifc in modules(directory):
        objects.append(interface(ifc, lib_prefix=lib_prefix))
    return [str(obj) for obj in objects]

def lib(name):
    objects = ' '.join(compile_directory(Path(name) / 'src', lib_prefix=name + '.'))
    lib = Path('build') / (name + '.lib')
    ninja.write(f'build {lib}: lib {objects}\n\n')
    return lib

def exe(name, directory, libs=[]):
    objects = ' '.join(compile_directory(Path(directory)))
    libs = ' '.join(str(lib) for lib in libs)
    exe = Path('build') / (name + '.exe')
    pdb = exe.with_suffix('.pdb')
    ilk = exe.with_suffix('.ilk')
    ninja.write(f'build {exe} | {pdb} {ilk}: link {objects} {libs}\n')
    ninja.write(f'    pdb = {pdb}\n')
    ninja.write('\n')

def synthesize(header):
    ifc = Path('build') / 'ifc' / (header + '.ifc')
    obj = Path('build') / (header + '.obj')
    ninja.write(f'build {ifc} | {obj}: synthesize\n')
    ninja.write(f'    header = {header}\n')
    ninja.write(f'    obj = {obj}\n')
    ninja.write('\n')
    return str(obj)

#endregion boilerplate

ninja.write('builddir = build\n\n')
ninja.write('include rules.ninja\n\n')

ninja.write('build build.ninja: configure configure.py\n')
ninja.write('    generator = 1\n\n')

utl = lib('utl')
windows = lib('windows')

exe('example', 'src', libs=[
    utl,
    windows,
])
