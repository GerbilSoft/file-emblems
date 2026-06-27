# File Emblems

This is a plugin for KDE Frameworks 6 that adds emblem functionality,
similar to what's available with GNOME's Nautilus and XFCE's Thunar.

[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)<br>
[![CodeFactor](https://www.codefactor.io/repository/github/gerbilsoft/file-emblems/badge)](https://www.codefactor.io/repository/github/gerbilsoft/file-emblems/overview)

###### no vibes, no bevis

## Not ready for primetime yet

Current status:
* KF6 plugin supports reading emblems from the `user.emblem` extended attribute.
* If built with ENABLE_GVFS, and `user.emblem` is not set, the plugin will also
  attempt to read emblems from GVfs metadata, i.e. emblems that were set with
  Nautilus or Thunar.

TODO:
* Sync GVfs emblems to XAttr if GVfs is set but XAttr is not.
* Similarly, sync XAttr to GVfs if XAttr is set but GVfs is not.
* If both are set, neither should be updated, unless you explicitly change
  them in the property page. (see the next item)
* Add a property page to allow selecting one or more emblems, which will be
  saved to the XAttr (and GVfs if enabled).
* Maybe add a KF5 plugin?

## Feedback

This is a work in progress; feedback is encouraged. To leave feedback, you
can file an issue on GitHub.

## License

File Emblems is licensed under the GNU General Public License v2.

## Credits

### Developers

* @GerbilSoft: Main developer.

### See Also

[ROM Properties Page Shell Extension](https://github.com/GerbilSoft/file-emblems),
another one of my projects. File Emblems is heavily based on parts of
rom-properties' code.
