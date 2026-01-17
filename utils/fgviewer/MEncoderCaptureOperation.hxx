/*
 * SPDX-FileName: MEncoderCaptureOperation.hxx
 * SPDX-FileComment: capture video stream into mencoder
 * SPDX-FileCopyrightText: Copyright (C) 2009 - 2012  Mathias Froehlich
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <cstdio>
#include <sstream>
#include <string>

#include <osgViewer/ViewerEventHandlers>

/// Class to capture into a pipe driven mencoder.
/// To integrate this into a viewer:
///  MEncoderCaptureOperation* mencoderCaptureOperation = new MEncoderCaptureOperation("/tmp/fgviewer.avi", 60);
///  osgViewer::ScreenCaptureHandler* c = new osgViewer::ScreenCaptureHandler(mencoderCaptureOperation, -1);
///  viewer.addEventHandler(c);
///  c->startCapture();

namespace fgviewer {

class MEncoderCaptureOperation : public osgViewer::ScreenCaptureHandler::CaptureOperation
{
public:
    MEncoderCaptureOperation(const std::string& fileName = "video.avi", unsigned fps = 30) : _fps(fps),
                                                                                             _fileName(fileName),
                                                                                             _options("-ovc lavc"),
                                                                                             _file(0),
                                                                                             _width(-1),
                                                                                             _height(-1)
    {
    }
    virtual ~MEncoderCaptureOperation()
    {
        _close();
    }

    const std::string& getFileName() const
    {
        return _fileName;
    }
    void setFileName(const std::string& fileName)
    {
        _fileName = fileName;
    }

    unsigned getFramesPerSecond() const
    {
        return _fps;
    }
    void setFramesPerSecond(unsigned fps)
    {
        _fps = fps;
    }

    const std::string& getOptions() const
    {
        return _options;
    }
    void setOptions(const std::string& options)
    {
        _options = options;
    }

    virtual void operator()(const osg::Image& image, const unsigned int)
    {
        // Delay any action until we have a valid image
        if (!image.valid())
            return;

        // Ensure an open file
        if (!_file) {
            // If the video was already opened and we got any error,
            // do not reopen with the same name.
            if (0 < _width)
                return;
            _width = image.s();
            _height = image.t();
            if (!_open())
                return;
        }
        // Ensure we did not change dimensions
        if (image.s() != _width)
            return;
        if (image.t() != _height)
            return;

        // Write upside down flipped image
        for (int row = _height - 1; 0 <= row; --row) {
            size_t ret = fwrite(image.data(0, row), 1, image.getRowSizeInBytes(), _file);
            if (ret != image.getRowSizeInBytes())
                return;
        }
    }

private:
    bool _open()
    {
        if (_file)
            return false;
        /// FIXME improve: adapt format to the format we get from the image
        std::stringstream ss;
        ss << "mencoder - -demuxer rawvideo -rawvideo fps="
           << _fps << ":w=" << _width << ":h=" << _height
           << ":format=rgb24 -o " << _fileName << " " << _options;
#ifdef _WIN32
        _file = _popen(ss.str().c_str(), "wb");
#else
        _file = popen(ss.str().c_str(), "w");
#endif
        return _file != 0;
    }
    void _close()
    {
        if (!_file)
            return;
#ifdef _WIN32
        _pclose(_file);
#else
        pclose(_file);
#endif
        _file = 0;
    }

    /// Externally given:
    unsigned _fps;
    std::string _fileName;
    std::string _options;

    /// Internal determined
    FILE* _file;
    int _width;
    int _height;
};

} // namespace fgviewer
