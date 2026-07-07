#include <iostream>

void And(int a, int b, int &out) {
  out = a && b;
}

void Nor(int a, int b, int &out) {
  out = !(a || b);
}

void TM(int a, int b, int &out) {
  if (b == 0) {
    out = a;
  } else {
    out = -1;
  }
}

void Not(int a, int &out) {
  out = !a;
}

int main() {
  for (int i = 0; i < 8; i++) {
    int a = (i >> 2) & 1;
    int b = (i >> 1) & 1;
    int c = (i >> 0) & 1;
    int na;
    int nb;
    int nc;
    Not(a, na);
    Not(b, nb);
    Not(c, nc);

    int i_tm1, i_tm2, i_tm3, i_tm4;
    TM(a, b, i_tm1);
    TM(na, nb, i_tm2);
    int i_tm12, i_tm34;
    if (i_tm1 == -1) {
      i_tm12 = i_tm2;
    } else {
      i_tm12 = i_tm1;
    }
    TM(c, i_tm12, i_tm3);
    int n_i_tm12;
    Not(i_tm12, n_i_tm12);
    TM(nc, n_i_tm12, i_tm4);
    if (i_tm3 == -1) {
      i_tm34 = i_tm4;
    } else {
      i_tm34 = i_tm3;
    }

    int i_and, i_or, i_nor, i_not;
    And(i_tm12, c, i_and);
    Nor(na, nb, i_nor);
    Nor(i_and, i_nor, i_or);
    Not(i_or, i_not);

    std::cout << i_tm34 << ", " << i_not << std::endl;
  }

  return 0;
}