[Producer(i:1..N)::produce || Consumer(j:1..M)::consume || Buffer::buffer]

produce::
*[
    int item = PRODUCE_ITEM;
    Buffer!put(item);
]

consume::
*[
    int item;
    Buffer!get();
    Buffer?item;
]

buffer::
[
    int buffer(0..B-1);  // niz od B elemenata
    int readIndex = 0, writeIndex = 0, size = 0;
    int item;

    *[
        size > 0, Consumer(j)?get() -> [
            Consumer(j)!buffer(readIndex);
            readIndex = (readIndex + 1) % B;
            size--;
        ]

        []

        size < B, Producer(i)?put(item) -> [
            buffer(writeIndex) = item;
            writeIndex = (writeIndex + 1) % B;
            size++;
        ]
    ]

]