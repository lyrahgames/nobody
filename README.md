# NOBODY

![](https://img.shields.io/github/languages/top/lyrahgames/nobody.svg?style=for-the-badge)
![](https://img.shields.io/github/languages/code-size/lyrahgames/nobody.svg?style=for-the-badge)
![](https://img.shields.io/github/repo-size/lyrahgames/nobody.svg?style=for-the-badge)
![](https://img.shields.io/github/license/lyrahgames/nobody.svg?style=for-the-badge)

<b>
<table>
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/lyrahgames/nobody">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/nobody/master.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/nobody/tree/master"><img src="https://circleci.com/gh/lyrahgames/nobody/tree/master.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/nobody">
              <img src="https://codecov.io/gh/lyrahgames/nobody/branch/master/graph/badge.svg" />
            </a>
        </td>
    </tr>
    <tr>
        <td>
            develop
        </td>
        <td>
            <a href="https://github.com/lyrahgames/nobody/tree/develop">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/nobody/develop.svg?logo=github&logoColor=white">
            </a>
        </td>    
        <td>
            <a href="https://circleci.com/gh/lyrahgames/nobody/tree/develop"><img src="https://circleci.com/gh/lyrahgames/nobody/tree/develop.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/nobody">
              <img src="https://codecov.io/gh/lyrahgames/nobody/branch/develop/graph/badge.svg" />
            </a>
        </td>
    </tr>
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/lyrahgames/nobody">
                <img src="https://img.shields.io/github/commit-activity/y/lyrahgames/nobody.svg?logo=github&logoColor=white">
            </a>
        </td>
        <td>
            <img src="https://img.shields.io/github/release/lyrahgames/nobody.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/lyrahgames/nobody.svg?label=pre-release&logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag/lyrahgames/nobody.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/lyrahgames/nobody.svg?label=latest%20tag&logo=github&logoColor=white">
        </td>
    </tr>
</table>
</b>

'nobody' is an n-body simulator.
It is still in development.

## Building
For downloading 'nobody' and starting with the build process do the following.

    git clone https://github.com/lyrahgames/nobody.git
    cd nobody
    mkdir build
    cd build
    cmake ..
    cmake --build .

## Usage
In the directory 'build' run the following.

    nobody/nobody

For some other data run the command with a file in the data directory.

    nobody/nobody ../data/<filename>

## Dependencies
- [OpenGL](https://www.opengl.org/)
- [GLUT](https://www.opengl.org/resources/libraries/glut/)
- [Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- OpenMP
- [doctest](https://github.com/onqtam/doctest)

## Authors
- Clemens Anschütz
- Markus Pawellek