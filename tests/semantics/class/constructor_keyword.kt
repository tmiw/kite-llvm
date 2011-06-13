class X [
    construct() [ this.elite = 1337|obj; 1; ];
    construct(val) [ this.elite = val|obj; 1; ];
];

x = make X();
x.elite|print;
x = make X(1234);
x.elite|print;
