run [
    (true != 2.0|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
