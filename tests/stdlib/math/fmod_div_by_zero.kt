import "System.math";

run
[
    System.math|fmod(3.0, 0.0)|print;
]
catch
[
    __exc.message|print;
];