/*
 * SPDX-FileName: Renderer.hxx
 * SPDX-FileCopyrightText: Copyright (C) 2009 - 2012  Mathias Froehlich
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <string>

#include <simgear/structure/SGWeakReferenced.hxx>

namespace fgviewer {

class Drawable;
class Viewer;
class SlaveCamera;

/// Default renderer, doing fixed function work
class Renderer : public SGWeakReferenced
{
public:
    Renderer();
    virtual ~Renderer();

    virtual Drawable* createDrawable(Viewer& viewer, const std::string& name);
    virtual SlaveCamera* createSlaveCamera(Viewer& viewer, const std::string& name);

    virtual bool realize(Viewer& viewer);
    virtual bool update(Viewer& viewer);

private:
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);

    class _SlaveCamera;
};

} // namespace fgviewer
