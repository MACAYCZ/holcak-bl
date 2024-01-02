# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased
Support for booting unpartitioned media with the chainload protocol.

### Added
- [ ] Switching between 16-bit and 32-bit mode
    - [ ] GAS macros for entering and leaving 16-bit mode
    - [ ] Function for calling BIOS interrupts from C
- [ ] Reading sectors from disks using BIOS interrupts
    - [ ] BIOS interrupt 0x13, ah=0x02
    - [ ] BIOS interrupt 0x13, ah=0x42
- [ ] Chainload protocol function
