// MIT License
//
// Copyright (c) 2023 Julia Nelz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "document.h"
#include "appui.h"

#include <akndoc.h>
#include <e32base.h>
#include <e32const.h>
#include <eikapp.h>
#include <eikdoc.h>

CSdlDocument::~CSdlDocument() {}

CSdlDocument::CSdlDocument(CEikApplication &aApp) : CAknDocument{aApp} {}

void CSdlDocument::ConstructL() {}

CSdlDocument *CSdlDocument::NewL(CEikApplication &aApp) {
  auto self = new (ELeave) CSdlDocument(aApp);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();

  return self;
}

CEikAppUi *CSdlDocument::CreateAppUiL() {
  return new (ELeave) CSdlAppUi;
}
