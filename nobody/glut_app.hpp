#ifndef NOBODY_GLUT_APP_H_
#define NOBODY_GLUT_APP_H_

// interface class for glut application functions
// this class forces correct order of glut functions
// this makes sense because glut uses non-member functions without any
// possibility for extra data input
// otherwise use singleton with static member functions
// explicit destructor is omitted because glut uses exit() and therefore the
// destructor will not be called after calling execute()
class Glut_app {
 public:
  Glut_app();
  Glut_app(int argc, char** argv);

  void execute();  // cannot be static, constructor has to be called first
};

#endif  // NOBODY_GLUT_APP_H_