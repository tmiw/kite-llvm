run [
    (2.0|obj == 2|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
