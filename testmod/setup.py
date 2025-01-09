from setuptools import Extension, setup
from snek import pkg_config


setup(
    name="testmod",
    version="0.0.0",
    ext_modules=[
        Extension(
            name="testmod",
            sources=["testmod.cc"],
            include_dirs=pkg_config()),
    ],
)