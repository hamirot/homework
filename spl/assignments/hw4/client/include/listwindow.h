#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include "../include/typedef.h"
#include "../include/utils.h"
#include "../include/contentwindow.h"

#include <iostream>
#include <sstream>
#include <istream>
#include <ostream>

template <typename T>
class ListWindow : public ContentWindow<T> {
    public:
        ListWindow(
                std::string name, 
                int height, 
                int width, 
                int starty, 
                int startx
                ) :
            ContentWindow<T>(name, height, width, starty, startx),
            _list(),
            _visibleSize(-1),
            _reverseList(true),
            _printToScreen(false)
        {
        }

        virtual void redraw() {
            this->clear();

            // count, from the bottom up, how many lines each item string
            // representation will require. get up to _visibleSize lines
            // and then spit them out at once.

            std::vector<std::string> outputLines;
            size_t linesCount = 0;

            if (_reverseList) {
                for (typename std::vector<T>::reverse_iterator it = _list.rbegin();
                        it != _list.rend() && linesCount < _visibleSize;
                        ++it)
                {
                    // wordwrap the text at _wrapWidth 
                    // and then split it to lines.
                    Strings lines = Utils::split(Utils::wordwrap((*it)->toString(), 150), '\n');

                    // reverse iterate on the lines
                    // and add them to the output lines.
                    for (Strings::reverse_iterator jj = lines.rbegin();
                            jj != lines.rend() && linesCount < _visibleSize;
                            ++jj) 
                    {
                        outputLines.push_back(*jj); 
                        linesCount++;
                    }
                }

                int ii = 0;
                for (Strings::reverse_iterator r_it = outputLines.rbegin();
                        r_it != outputLines.rend();
                        ++r_it, ++ii)
                {
                    this->print(this->getOffsetY() + ii, this->getOffsetX(), (*r_it));
                } 
            } else {
                for (typename std::vector<T>::iterator it = _list.begin();
                        it != _list.end() && linesCount < _visibleSize;
                        ++it)
                {
                    // wordwrap the text at _wrapWidth 
                    // and then split it to lines.
                    Strings lines = Utils::split(Utils::wordwrap((*it)->toString(), 150), '\n');

                    // iterate on the lines
                    // and add them to the output lines.
                    for (Strings::iterator jj = lines.begin();
                            jj != lines.end() && linesCount < _visibleSize;
                            ++jj) 
                    {
                        outputLines.push_back(*jj); 
                        linesCount++;
                    }
                }

                int ii = 0;
                for (Strings::iterator it = outputLines.begin();
                        it != outputLines.end();
                        ++it, ++ii)
                {
                    this->print(this->getOffsetY() + ii, this->getOffsetX(), (*it));
                } 
            }

            this->refreshWindow();
        };

        virtual void setPrintToScreen(bool printToScreen) {
            this->_printToScreen = printToScreen;  
        };

        /**
         * Add an item to the list.
         **/
        virtual void addItem(T item) {
            if (this->_printToScreen) {
                std::cout << '\r' << item->toString() << std::endl << std::flush;
            }

            _list.push_back(item);

            this->redraw();
        };

        /**
         * Add a list of items.
         **/
        virtual void addItems(std::vector<T> items) {
            for (typename std::vector<T>::iterator it = items.begin();
                    it != items.end();
                    ++it)
            {
                _list.push_back(*it);
            }

            this->redraw();
        };

        virtual void setItem(int index, T item) {
            _list.at(index) = item;

            this->redraw();
        };

        virtual void removeItem(int index) {
            _list.erase(_list.begin() + index);

            this->redraw();
        };

        virtual void removeAll() {
            _list.clear();

            this->redraw();
        };

        virtual std::vector<T> getList() {
            return _list;
        };

        virtual void setVisibleSize(size_t size) {
            _visibleSize = size;
        };

        virtual void setReverseList(bool reverseList) {
            _reverseList = reverseList;
        };  

        virtual size_t size() {
            return _list.size();
        };

    protected:
        std::vector<T> _list;
        size_t _visibleSize;
        bool _reverseList;
        bool _printToScreen;
};

#endif
