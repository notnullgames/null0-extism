This is a version of null0 host that uses [extism](https://extism.org/).

To get started, install [extism](https://extism.org/):

```sh
curl -s https://get.extism.org/cli | sh
```

For some cart-languages, you will need to install the libraries. For example, this is for C:

```
sudo extism lib install
```

To make a new cart:

```
extism generate plugin -o mycart
```

The final cart format is a zip file, with a main.wasm, and any assets you want to include.