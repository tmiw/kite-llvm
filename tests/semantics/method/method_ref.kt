method x(a) [ a|print; ];
method x(a, b) [ (a+b)|print; ];
method x() [ 31337|print; ];

v = method_ref(this, "x", 0);
v();
v = method_ref(this, "x", 1);
v(1);
v = method_ref(this, "x", 2);
v(1, 2);