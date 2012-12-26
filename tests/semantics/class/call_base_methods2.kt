class X [
    construct(name) [ "exec X"|print; this.n = name; ] 
];

class Y from X [
    construct(name) [ "exec Y"|print; base|__init__(name); ]
];

class Z from Y [
    construct(name) [ "exec Z"|print; base|__init__(name); ]
];

v = make Z("xxx");
v.n|print;
