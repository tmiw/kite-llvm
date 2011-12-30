class X [
    construct() [ this.y = 31336; ];
    method new_method()
    [
        (method() [ (this.y + 1); ]);
    ];
];

y = 0;
z = make X();
v = z|new_method;
v()|print;