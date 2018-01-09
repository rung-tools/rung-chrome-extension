# Rung Chrome Extension

![Logo](./resources/logo.png)

This is the official extension for Chrome (and Firefox compatible) for Rung, the exceptionalities manager.

![Screenshot](./resources/demo.png)

The extension is currently available in portuguese, english, french and spanish. We'll add more languages soon!

## Contributing

All kinds of contributions are welcome! We really are into open-source and we love it.

### Compiling the project

To compile locally this project you we need the following tools:

- OCaml 4.0+
- Opam 4.02.3
- BuckleScript with ReasonML

To run this project, after cloning, in the folder do:

```shell
npm install
npm start
# in another tab
npm run webpack
```

To test the extension, just open Chrome, go to extensions, enable developer mode and upload the folder
with the resources. You can do live development by letting BuckleScript and Webpack watch your source.

> Proudly built in OCaml + Reason and React

Copyright 2018 Rung · All rights reserved
