class test [
    method hello() [
        "hi"|print;
    ];
    
    method greetings() [
        eval "this|hello;";
    ];
];

v = make test();
v|greetings;