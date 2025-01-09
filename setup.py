from setuptools import Extension, setup

setup(
    name="snek",
    version="0.0.0",
    packages=["snek"],  
    package_data={
        "snek": ["include/**/*"]
    },
    include_package_data=True,
)