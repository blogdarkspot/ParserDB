from distutils.core import setup, Extension

module1 = Extension('pcfg',
    define_macros=[('MAJOR_VERSION', '1'), ('MINOR_VERSION', '0')],
    include_dirs=['/usr/local/include', '/usr/include', '../../include'],
    sources=['../../src/export/pcfg.cpp'])

setup(name='Gramatico', 
        version='1.0', 
        description='Engine para analise de linguagem natural', 
        author='Fabio da Silva Santana', 
        author_email='fabio@fabio.com', 
        url='github.com', 
        long_description= """ 
        
        """, ext_modules= [module1])

