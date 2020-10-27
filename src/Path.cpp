#include "../include/slib/Path.hpp"
#include "../include/slib/List.hpp"
#include "../include/slib/LinkedList.hpp"
#include <stdio.h>
#define _AMD64_
#include <debugapi.h>

namespace slib {

    bool IsSeparator(char c) {
        return c == '/' || c == '\\';
    }

    bool StartsWithDrive(String pathStr) {
        return pathStr[1] == ':' && IsSeparator(pathStr[2]);
    }

    Path::Path(String pathStr) : pathStr(pathStr) {}

    bool Path::IsAbsolute() const {
#ifdef _WIN32
        // On Windows, there are two kinds of absolute path.
        // The normal kind start with a drive letter, followed by ':' and finally '\' or '/'.
        // The other kind, UNC paths, start with two backslashes.
        // For now, however, we'll just pretend that UNC paths don't exist since we don't need them.
        // Therefore the shortest valid path will have at least 3 characters.

        if (pathStr.ByteLength() < 3) return false;

        // Drive letter path
        if (pathStr[1] == ':' && IsSeparator(pathStr[2])) return true;

        return false;
#else
        // On Linux, an absolute path will always start with a forward slash.
        return pathStr.ByteLength() > 0 && pathStr[0] == '/';
#endif
    }

    String Path::FileExtension() {
        String::Iterator lastDot = pathStr.End();

        for (auto it = pathStr.Begin(); it < pathStr.End(); ++it) {
            if (*it == '.') lastDot = it;
        }

        if (lastDot == pathStr.End()) return "";

        return pathStr.Substring(lastDot.GetIndex());
    }

    String Path::Stem() {
        String::Iterator lastSlash = pathStr.End();
        String::Iterator lastDot = pathStr.End();

        for (auto it = pathStr.Begin(); it < pathStr.End(); ++it) {
            if (IsSeparator(*it)) lastSlash = it;
            if (*it == '.') lastDot = it;
        }

        size_t start = lastSlash == pathStr.End() ? 0 : lastSlash.GetIndex() + 1;

        return pathStr.Substring(start, lastDot.GetIndex() - start);
    }

    void Path::Normalize() {
        // Empty path is already normalized
        if (pathStr.Empty()) return;

        bool hasRootDrive = pathStr[1] == ':' && IsSeparator(pathStr[2]);

        // Build a linked list of path elements for easy removal
        LinkedList<String> elements;

        pathStr += "/";

        String curr;
        for (auto it = pathStr.Begin(); it < pathStr.End(); ++it) {
            if (IsSeparator(*it)) {
                // Ignore multiple separators by skipping when we don't have a name built up
                if (curr.Empty()) continue;

                elements.Add(curr);

                curr.Clear();
                continue;
            }
            curr += *it;
        }

        using Element = LinkedList<String>::Element;
        // Go through the path's linked list form and remove unnecessary parts
        // (i.e. '.'s and 'blah/..')
        {
            Element* next = elements.First();
            while (next != nullptr) {
                Element* c = next;

                next = c->next;

                // Remove "."
                if (c->v == ".") {
                    elements.Remove(c);
                    continue;
                }

                // Remove .. and the directory before it if there's a directory name preceding it
                if (c->v == ".." && c->prev) {
                    Element* a = c;
                    Element* b = c->prev;

                    elements.Remove(a);
                    elements.Remove(b);
                }
            }
        }


        // Recreate path from linked list
        String newPath;

        if (IsSeparator(pathStr[0]))
            newPath += '/';

        /*if (hasRootDrive) {
            newPath = pathStr.Substring(0, 2) + "/";
        }*/

        Element* next = elements.First();
        while (next != nullptr) {
            Element* c = next;

            newPath += c->v;
            newPath += '/';

            next = c->next;
        }

        // Strip trailing separator
        if (newPath[newPath.ByteLength() - 1]) {
            newPath = newPath.Substring(0, newPath.ByteLength() - 1);
        }

        pathStr = newPath;
    }

    Path Path::ParentPath() const {
        Path dup(pathStr);
        dup.Normalize();

        // Search for the last directory separator and take the path
        // to be everything before that

        String::Iterator lastSlash = dup.pathStr.End();

        for (auto it = dup.pathStr.Begin(); it < dup.pathStr.End(); ++it) {
            if (IsSeparator(*it)) lastSlash = it;
        }

        dup.pathStr = dup.pathStr.Substring(0, lastSlash.GetIndex());

        // Add the slash for the drive path
        if (StartsWithDrive(pathStr) && dup.pathStr.ByteLength() == 2) {
            dup.pathStr += "/";
        }

        return dup;
    }

    const char* Path::CStr() const {
        return pathStr.CStr();
    }

    void Path::ReplaceSeparator(char newSeparator) {
        for (auto& c : pathStr) {
            if (IsSeparator(c))
                c = newSeparator;
        }
    }

    Path::operator String() const{
        return pathStr;
    }
}