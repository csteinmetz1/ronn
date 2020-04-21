from distutils.core import setup

setup(name='disnet',
      version='0.0.1',
      description='DISNET development pacakge',
      author='Christian Steinmetz',
      author_email='cjstein@clemson.edu',
      url='https://www.github.com/csteinmetz1/distnet/',
      packages=['disnet'],
      install_requires=['future',
                        'torch',
                        'torchaudio',
                        'torchsummary']
     )