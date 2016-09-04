# nbddapigen

nbddapigen, NBDrupalDevel API Generator, is a tool for generating the required
API docs for use with NBDrupalDevel.


## License Information

vendor/grammar-parser is licensed under the GPLv2 and was bundled from sources that
can be found under http://git.drupal.org/project/grammar_parser.git.


## Intended Audience

All Netbeans/NDDrupalDevel users who are in desperate need of newer API docs
for their favourite CMS and for the plugins you use.


## Installation

There are multiple paths here:

1. Checkout the repository and build the distribution yourself, or
2. Download and install the latest releases directly from https://github.com/coldrye-php/nbddapigen/releases.


## Usage

When generating the documentation for a new Drupal release, you simply use

```
> nbddapigen -d <version> <drupal-root>
> mv ./nbddapigenout/<version> ~/.netbeans/<version>/DrupalDevel/code/
```
