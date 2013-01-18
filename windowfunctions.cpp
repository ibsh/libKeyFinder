/*************************************************************************

  Copyright 2011-2013 Ibrahim Sha'ath

  This file is part of LibKeyFinder.

  LibKeyFinder is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LibKeyFinder is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************************/

#include "windowfunctions.h"

namespace KeyFinder{

  WindowFunction* WindowFunction::getWindowFunction(temporal_window_t w){
    if(w == WINDOW_HANN){
      return new HannWindow();
    }else if(w == WINDOW_HAMMING){
      return new HammingWindow();
    }else{
      return new BlackmanWindow();
    }
  }

  // subclasses

  float HannWindow::window(int n, int N)const{
    return 0.5 * (1.0 - cos((2 * PI * n)/(N-1)));
  }

  float HammingWindow::window(int n, int N)const{
    return 0.54 - (0.46 * cos((2 * PI * n)/(N-1)));
  }

  float BlackmanWindow::window(int n, int N)const{
    return 0.42 - (0.5 * cos((2 * PI * n)/(N-1))) + (0.08 * cos((4 * PI * n)/(N-1)));
  }

}
