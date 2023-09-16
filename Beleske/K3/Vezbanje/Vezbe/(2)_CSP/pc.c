[Producer(i:1..N)::produce || Consumer(j:1..M)::consume || Buffer::buffer]

produce::
[
    int data = ...;
    Buffer!put(data);
]

consume::
[
    int data;
    Buffer!get();
    Buffer?data;
]

buffer::
[
    int buffer[B];
    int readIndex = 0, writeIndex = 0, size = 0;
    int data;

    * [
        size > 0, Consumer(j)?get() -> [
            Consumer(j)!buffer[readIndex];
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