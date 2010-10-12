# hornetseye-opencv - OpenCV integration for Hornetseye
# Copyright (C) 2010 Jan Wedekind
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
module OpenCV

  class CvMat

    alias_method :orig_to_multiarray, :to_multiarray

    def to_multiarray
      retval = orig_to_multiarray
      case channel
      when 1
        retval.roll[ 0 ]
      when 2
        Hornetseye::MultiArray( Hornetseye::COMPLEX( retval.typecode ),
                                width, height ).new retval.memory
      when 3
        # swap rgb!
        Hornetseye::MultiArray( Hornetseye::RGB( retval.typecode ),
                                width, height ).new retval.memory
      else
        retval
      end
    end

  end

end

