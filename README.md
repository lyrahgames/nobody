# NOBODY

'nobody' is an n-body simulator.
It is still in development and therefore it can contain some bugs.

## Building
For downloading 'nobody' and starting with the build process do the following.

    git clone https://github.com/lyrahgames/nobody.git
    cd nobody
    mkdir build
    cd build
    cmake ..
    make

## Usage
In the directory 'build' run the following.

    ./nobody

For some other data run the command with a file in the data directory.

    ./nobody ../data/<filename>

## Dependencies
- [OpenGL](https://www.opengl.org/)
- [GLUT](https://www.opengl.org/resources/libraries/glut/)
- [Eigen 3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

## Contributing
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Git Flow](http://nvie.com/posts/a-successful-git-branching-model/)
- [Git Commit Messages](https://chris.beams.io/posts/git-commit/)

## Authors
- Clemens Anschütz
- Markus Pawellek