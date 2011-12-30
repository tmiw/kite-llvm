i = 0;

while (i < 10) [
    decide [
        (i == 5) [ i = i + 1; continue; ],
        true [ i|print; ]
    ];
    i = i + 1;
];
