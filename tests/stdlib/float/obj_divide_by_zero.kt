run [
    (6.0|obj / 0.0|obj)|print;
] catch [
    __exc.__name|print;
    __exc.message|print;
];
