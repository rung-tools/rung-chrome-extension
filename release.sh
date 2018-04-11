#!/usr/bin/env bash
rm -rf public
yarn build
yarn webpack:release
cd public
zip -r rung.crx .
cd ..
