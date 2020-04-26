# RONN research and development
This is a research and development version of DISNET where we explore some ideas 
using PyTorch, and additionally provide this as a Python package that
could easily be incorporated into other Python projects. 

## Setup

From this directory, install RONN as a package,

```bash
python setup.py install 
```

or install directly from GitHub.

```bash
pip install git+https://github.com/csteinmetz/RONN/dev/RONN
```

## Usage

It has a very simple API to create some distortions, and is designed
in a block-based fashion as it would operate in a real-time plugin.

```python
import ronn
```