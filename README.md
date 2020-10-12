# Embedded ML
[![Build Status](https://travis-ci.org/LesleyLai/eml.svg?branch=master)](https://travis-ci.org/LesleyLai/eml)
[![Build status](https://ci.appveyor.com/api/projects/status/rf9t1bnli5rpj5r0/branch/master?svg=true)](https://ci.appveyor.com/project/LesleyLai/eml/branch/master)
[![codecov](https://codecov.io/gh/LesleyLai/eml/branch/master/graph/badge.svg)](https://codecov.io/gh/LesleyLai/eml)

Embedded ML aims to be a static-typed scripting language that gets performance and type safety of a static-typed language, while still remains most of the convenience of a dynamic-typed language. This project is in a very early stage, so I will make breaking changes frequently.


## Quick Start
See how to build the project [here](docs/src/build.md).

## Acknowledgements

- Thanks to [Sy Brand](https://blog.tartanllama.xyz/) for their [tl::expected](https://github.com/TartanLlama/expected).
- Thanks to [Clare Macrae](https://claremacrae.co.uk/) and [Llewellyn Falco](https://llewellynfalco.blogspot.com/p/infographics.html)
for [ApprovalTests.cpp](https://github.com/approvals/ApprovalTests.cpp) and the idea of Approval Test in general

## Examples

Here are some snippets of what an EML program looks like and what it evaluate to:

### If-else
snippet: Else_If.approved.eml

evaluates into:

snippet: Else_If.approved.docs.txt