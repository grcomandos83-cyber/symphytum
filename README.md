[![Release](https://github.com/grcomandos83-cyber/symphytum-database/actions/workflows/release.yml/badge.svg)](https://github.com/grcomandos83-cyber/symphytum-database/actions/workflows/release.yml)
[![License](https://img.shields.io/badge/License-BSD_2--Clause-blue.svg)](https://opensource.org/licenses/BSD-2-Clause)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/grcomandos83-cyber/symphytum-database)](https://github.com/grcomandos83-cyber/symphytum-database/releases/latest)

![logo](https://raw.githubusercontent.com/grcomandos83-cyber/symphytum-database/master/stuff/logo/symphytum_64.png "Symphytum")
# Symphytum Database

> **Project Status & Maintenance Notice:**  
> This repository is an active continuation, modernization, and maintenance fork of the original [Symphytum](https://github.com/giowck/symphytum) personal database application following the archiving of the original repository. Development, modern Qt updates, fixes, and release packaging are actively maintained by [grcomandos83-cyber](https://github.com/grcomandos83-cyber).

Symphytum is a personal database software for everyone who desires to manage and organize data in an easy and intuitive way, without having to study complex database languages and software user interfaces.        

**Table of Contents**
* [Introduction](#introduction)
    * [Features](#features)
    * [How it Looks](#how-it-looks)
* [Download](#download)
    * [Windows](#windows)
        * [Installer](#1-windows-installer-setup-exe)
        * [Portable ZIP Archive](#2-portable-zip-archive-pre-compiled--ready-to-run)
    * [macOS](#macos)
        * [Disk Image (.dmg)](#1-macos-disk-image-dmg)
        * [Portable ZIP Bundle](#2-macos-portable-zip-bundle)
    * [Linux](#linux)
        * [Portable Tarball (.tar.gz)](#1-linux-portable-archive-targz)
* [Build from Source](#build-from-source)
* [Contribute](#contribute)
* [License](#license)

## Introduction
Symphytum is a free and open-source personal database software written in C++ and Qt for Windows, macOS and Linux. Design and edit simple databases in a visual and intuitive way, without any need to study complex database languages. Symphytum is directed at users who just want to organize data in custom designed collections without giving up advantages of database engines like fast loading speed, large data set handling, fast searching, sorting and more.

Manage all kind of data ranging from contacts, inventory, any type of collection, customers and so on. There are limitations though, Symphytum is not able to handle relational data and automatic field calculations yet. Also the import from CSV files is very limited, all imported data sets are handled just as text fields, future improvements to the import dialog, to specify each field type, are planned though.

Symphytum is able to synchronise your data through different cloud services like Dropbox or MEGA. It can detect and handle sync conflicts in case multiple user are using the same database via a supported cloud service.

Technically Symphytum is powered by the SQLite database engine, which is the leading embedded database solution, used in many mobile apps and modern computer programs, like web browsers, media players and email clients.
SQLite is tiny, efficient and very fast. It can handle huge amount of data while being highly resistant to data corruption. 

### Features
* **Fields Are Not Just Text**. Design your input forms with support for different data types: text, numeric, date, progress, image, file list, checkbox, combobox, etc.
* **Two Views On The Same Data**. Use the form view for structured data input and representation, use the table-like view for searching, sorting and comparing.
* **Dynamic Layout Engine**. Rearrange dynamically your database layout by drag and drop in form view.
* **Integrated Cloud Sync**. Using Symphytum across multiple computers is a joy. Your data is always automatically synchronised everywhere. Drivers for cloud services such as Dropbox and MEGA are included.
* **Sync Conflict Management**. Symphytum manages synchronisation conflicts for you. While only one session with write access is allowed at the same time, other computers may access the database in read-only mode during an open session.
* **Date Reminder**. Date fields keep you informed on tasks, appointments or birthdays, if requested. All Reminders, once triggered, are listed in one place.
* **Fast Search**. Search while typing with highlighted results in a table view.
* **Backup and Export**. Backup your data with a simple backup wizard and export your data to CSV.
* **Multilingual**. User interface available in English, German, Polish, French, Italian and Greek.


### How it Looks
Some screenshots showing the form view, table view, dynamic layout engine and the field addition dialog.

![form_view_img](https://raw.githubusercontent.com/grcomandos83-cyber/symphytum-database/master/stuff/screenshots/mainwindow.png "Form view")
![table_view_img](https://raw.githubusercontent.com/grcomandos83-cyber/symphytum-database/master/stuff/screenshots/tablieview.png "Table view")
![dynamic_layout_img](https://raw.githubusercontent.com/grcomandos83-cyber/symphytum-database/master/stuff/screenshots/dynamic_layout.gif "Dynamic layout engine")

![add_field_img](https://raw.githubusercontent.com/grcomandos83-cyber/symphytum-database/master/stuff/screenshots/addfield.png "Add field")

## Download
Please see the appropriate download section for your operating system below.
General releases, source archives and other info can be found on the [releases](https://github.com/grcomandos83-cyber/symphytum-database/releases) page. Thank you for downloading Symphytum.

### Windows
For Windows 7, 8, 10, and 11 (64-bit)

#### 1. Windows Installer (Setup .exe)
Standard Windows installer with desktop shortcut and uninstaller support:

Download [symphytum-2.7.1-setup.exe](https://github.com/grcomandos83-cyber/symphytum-database/releases/download/v2.7.1/symphytum-2.7.1-setup.exe)

#### 2. Portable ZIP Archive (Pre-compiled & Ready to Run)
A portable ZIP for Windows is self-contained: once extracted, it can be moved and launched on any machine without installation. The personal data is contained inside the folder alongside the main executable (`symphytum.exe`).

Download [Symphytum-2.7.1-Windows-Portable.zip](https://github.com/grcomandos83-cyber/symphytum-database/releases/download/v2.7.1/Symphytum-2.7.1-Windows-Portable.zip)

---

### macOS
For macOS 11 Big Sur, 12 Monterey, 13 Ventura, 14 Sonoma, 15 Sequoia (Intel & Apple Silicon via Rosetta 2)

#### 1. macOS Disk Image (.dmg)
Standard macOS drag-and-drop installer disk image:

Download [Symphytum-2.7.1-macOS.dmg](https://github.com/grcomandos83-cyber/symphytum-database/releases/download/v2.7.1/Symphytum-2.7.1-macOS.dmg)

#### 2. macOS Portable ZIP Bundle
Standalone application bundle archive (`Symphytum.app`):

Download [Symphytum-2.7.1-macOS.zip](https://github.com/grcomandos83-cyber/symphytum-database/releases/download/v2.7.1/Symphytum-2.7.1-macOS.zip)

---

### Linux
For modern 64-bit Linux distributions (Ubuntu, Debian, Fedora, Arch, etc.)

#### 1. Linux Portable Archive (.tar.gz)
Self-contained pre-compiled binaries and desktop integration resources:

Download [Symphytum-2.7.1-Linux-x86_64.tar.gz](https://github.com/grcomandos83-cyber/symphytum-database/releases/download/v2.7.1/Symphytum-2.7.1-Linux-x86_64.tar.gz)

## Build from Source

### Using CMake (Recommended)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Using qmake
```bash
cd symphytum
qmake -config release
make
```
*On Windows, you can also run `Build.bat` to automatically build and deploy all dependencies.*

## Contribute
Report an issue, bug or feature proposal at the [project's issue tracker](https://github.com/grcomandos83-cyber/symphytum-database/issues).

## License
Symphytum is licensed under the BSD 2-Clause License, see [LICENSE](https://github.com/grcomandos83-cyber/symphytum-database/blob/master/LICENSE). 
You can use Symphytum for free and for any purpose.

Copyright (c) 2026 grcomandos83-cyber  
Copyright (c) 2014-2020 Symphytum Developers  
Copyright (c) 2012-2014 GIOWISYS Software UG
