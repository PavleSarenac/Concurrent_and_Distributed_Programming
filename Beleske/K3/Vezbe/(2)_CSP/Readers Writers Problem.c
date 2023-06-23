// Resenje u kojem citaoci imaju prednost

[R(i:1..numberOfReaders)::reader || W(i:1..numberOfWriters)::writer || RW::buffer]

buffer::
[
    int rCnt = 0, wCnt = 0;
    * [
        wCnt == 0, R(i)?startRead() -> [
            rCnt++;
        ]

        [] 
        
        R(i)?endRead() -> [
            rCnt--;
        ]
        
        [] 
        
        wCnt == 0 and rCnt == 0, W(i)?startWrite() -> [
            wCnt++;
        ]

        [] 
        
        W(i)?endWrite() -> [
            wCnt--;
        ]
    ]
]

reader::
[
    RW!startRead();
    read();
    RW!endRead();
]
writer::
[
    RW!startWrite();
    write();
    RW!endWrite();
]
    
// Resenje koje je malo vise fer prema piscima
// Ukoliko dodje neki pisac koji zeli da pise, onda se ceka da svi trenutni citaoci zavrse citanje i onda ce pisac dobiti dozvolu
// da pise (nece seza to vreme prihvatati novi citaoci koji zele da citaju).

[R(i:1..numberOfReaders)::reader || W(i:1..numberOfWriters)::writer || RW::buffer]

buffer::
[
    int rCnt = 0;
    * [
        R(i)?startRead() -> [
            rCnt++;
        ]

        [] 
        
        R(i)?endRead() -> [
            rCnt--;
        ]

        [] 
        
        W(i)?startWrite() -> [
            * [
                rCnt > 0, R(j)?endRead() -> [
                    rCnt--;
                ]
            ];
            W(i)!ok();
            W(i)?endWrite();
        ]
    ]
]

reader::
[
    RW!startRead();
    read();
    RW!endRead();
]

writer::
[
    RW!startWrite();
    RW?ok();
    write();
    RW!endWrite();
]