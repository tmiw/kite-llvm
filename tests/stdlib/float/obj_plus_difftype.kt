run [
    (1.0|obj + 3|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];