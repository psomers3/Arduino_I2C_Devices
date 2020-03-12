from setuptools import setup, find_packages


#with open("README.md", "r") as fh:
#    long_description = fh.read()

setup(
    name="ArduinoI2C",
    version="0.0.1",
    author="Peter Somers",
    author_email="st164174@stud.uni-stuttgart.com",
    description="A Python module to use Arduino as I2C device",
    long_description='long_description',
    long_description_content_type="text/markdown",
    url="https://github.tik.uni-stuttgart.de/psomers/Arduino_I2C_Devices",
    packages='.',
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires='>=3',
)
