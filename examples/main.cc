#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

void hoge() {
  cout << "hoge called" << endl;
}

void fuga(const char* str) {
  cout << "fuga called: " << str << endl;
}

std::string piyo(const std::string& s) {
  return s + s;
}

int square(int x) {
  return x * x;
}

double add(double x, double y) {
  return x + y;
}

void test(bool t) {
  cout << t << endl;
}


class Hoge {
public:
  Hoge(int x) : x_(x) {
    cout << "Hoge::ctor(" << x << ")" << endl;
  }
  virtual ~Hoge() {
    cout << "Hoge::dtor()" << endl;
  }
  int hoge(int y) {
    return x_ + y;
  }

private:
  int x_;
};

void* new_hoge(int x) {
  return (void*)new Hoge(x);
}



int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("hoge", hoge);
    b.bind("fuga", fuga);
    b.bind("piyo", piyo);
    b.bind("square", square);
    b.bind("add", add);
    b.bind("test", test);
  }
  mrb_load_string(mrb,
                  "hoge()\n"
                  "fuga('piyo')\n"
                  "p piyo('abc')\n"
                  "p test('')\n"
                  "p square(1111)\n"
                  "p add(1.23, 9.87)\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }

  {
    mrubybind::MrubyBind b(mrb);
    b.define_class("Hoge", new_hoge);
    b.define_class_method("Hoge", "hoge", &Hoge::hoge);
  }
  mrb_load_string(mrb,
                  "h = Hoge.new(111)\n"
                  "p h.hoge(567)\n"
                  );

  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }

  mrb_close(mrb);
  return 0;
}
