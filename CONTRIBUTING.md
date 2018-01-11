## Contributing

Additions to the master branch must go through a pull request. The pull request must be approved by at least one reviewer and go through a Continuous Integration (CI) check by Travis. The CI only lints.


```
# recommended to set up a virtualenv to avoid polluting your system with pip
# first install virtualenv with your package manager, then
virtualenv venv
source venv/bin/activate
# run linting commands, e.g. cpplint, pep8
# exit virtualenv
deactivate
```

### C++

C++ code has to conform to Google's c++ [style guide](https://google.github.io/styleguide/cppguide.html). It is verified by the cpplint tool. To run it on your own code:

```
# install cpplint
pip install cpplint
# run cpplint, it checks all .cpp and .hpp files
cpplint --linelength=200 --filter=-legal/copyright,-build/c++11 --extensions=cpp,hpp --headers=hpp $(find . -name \*.hpp -or -name \*.cpp)
# resolve all errors
```

#### Names

Snake case for everything except class names. Class names use camel case.

#### Spacing

Indentation level is two spaces.

#### Pointers

```
int *bar
```

NOT

```
int* bar
```


### Python

Python code has to conform to the pep8 [style guide](https://www.python.org/dev/peps/pep-0008/). It is verified by the pep8 tool. To run it on your own code:

```
# install pep8
pip install pep8
# run pep8, it checks all .py files
pep8 --max-line-length=120 --exclude=venv .
# resolve all errors
```

## Pull requests

Merge and squash is used for pull requests.
