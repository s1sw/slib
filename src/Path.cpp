#include "../include/slib/Path.hpp"
#include "../include/slib/List.hpp"
#include "../include/slib/LinkedList.hpp"
#include <stdio.h>
#ifdef _WIN32
#define _AMD64_
#include <debugapi.h>
#endif

namespace slib {

    bool isSeparator(char c) {
        return c == '/' || c == '\\';
    }

    bool startsWithDrive(String pathStr) {
        return pathStr[1] == ':' && isSeparator(pathStr[2]);
    }

    Path::Path(String pathStr) : pathStr(pathStr) {}
    Path::Path(const Path& other) : pathStr(other.pathStr) {}

    bool Path::isAbsolute() const {
#ifdef _WIN32
        // On Windows, there are two kinds of absolute path.
        // The normal kind start with a drive letter, followed by ':' and finally '\' or '/'.
        // The other kind, UNC paths, start with two backslashes.
        // For now, however, we'll just pretend that UNC paths don't exist since we don't need them.
        // Therefore the shortest valid path will have at least 3 characters.

        if (pathStr.byteLength() < 3) return false;

        // Drive letter path
        if (pathStr[1] == ':' && isSeparator(pathStr[2])) return true;

        return false;
#else
        // On Linux, an absolute path will always start with a forward slash.
        return pathStr.byteLength() > 0 && pathStr[0] == '/';
#endif
    }

    String Path::fileExtension() {
        auto lastDot = pathStr.end();

        for (auto it = pathStr.begin(); it < pathStr.end(); ++it) {
            if (*it == '.') lastDot = it;
        }

        if (lastDot == pathStr.end()) return "";

        return pathStr.substring(lastDot.getIndex());
    }

    String Path::stem() {
        String::Iterator lastSlash = pathStr.end();
        String::Iterator lastDot = pathStr.end();

        for (auto it = pathStr.begin(); it < pathStr.end(); ++it) {
            if (isSeparator(*it)) lastSlash = it;
            if (*it == '.') lastDot = it;
        }

        size_t start = lastSlash == pathStr.end() ? 0 : lastSlash.getIndex() + 1;

        return pathStr.substring(start, lastDot.getIndex() - start);
    }

    void Path::normalize() {
        // Empty path is already normalized
        if (pathStr.empty()) return;


        // Build a linked list of path elements for easy removal
        LinkedList<String> elements;

        pathStr += "/";

        String curr;
        for (auto it = pathStr.begin(); it < pathStr.end(); ++it) {
            if (isSeparator(*it)) {
                // Ignore multiple separators by skipping when we don't have a name built up
                if (curr.empty()) continue;

                elements.add(curr);

                curr.clear();
                continue;
            }
            curr += *it;
        }

        using Element = LinkedList<String>::Element;
        // Go through the path's linked list form and remove unnecessary parts
        // (i.e. '.'s and 'blah/..')
        {
            Element* next = elements.first();
            while (next != nullptr) {
                Element* c = next;

                next = c->next;

                // Remove "."
                if (c->v == ".") {
                    elements.remove(c);
                    continue;
                }

                // Remove .. and the directory before it if there's a directory name preceding it
                if (c->v == ".." && c->prev) {
                    Element* a = c;
                    Element* b = c->prev;

                    elements.remove(a);
                    elements.remove(b);
                }
            }
        }


        // Recreate path from linked list
        String newPath;

        if (isSeparator(pathStr[0]))
            newPath += '/';

        Element* next = elements.first();
        while (next != nullptr) {
            Element* c = next;

            newPath += c->v;
            newPath += '/';

            next = c->next;
        }

        // Strip trailing separator
        if (newPath[newPath.byteLength() - 1]) {
            newPath = newPath.substring(0, newPath.byteLength() - 1);
        }

        pathStr = newPath;
    }

    Path Path::parentPath() const {
        Path dup(pathStr);
        dup.normalize();

        // Search for the last directory separator and take the path
        // to be everything before that

        String::Iterator lastSlash = dup.pathStr.end();

        for (auto it = dup.pathStr.begin(); it < dup.pathStr.end(); ++it) {
            if (isSeparator(*it)) lastSlash = it;
        }

        dup.pathStr = dup.pathStr.substring(0, lastSlash.getIndex());

        // Add the slash for the drive path
        if (startsWithDrive(pathStr) && dup.pathStr.byteLength() == 2) {
            dup.pathStr += "/";
        }

        return dup;
    }

    const char* Path::cStr() const {
        return pathStr.cStr();
    }

    void Path::replaceSeparator(char newSeparator) {
        for (auto& c : pathStr) {
            if (isSeparator(c))
                c = newSeparator;
        }
    }

    Path::operator String() const{
        return pathStr;
    }
}
