var data = 123;

function add(a, b) {
    return a + b;
}

exports = { file: __filename,
            dir: __dirname,
            data: data,
            add: add
};
