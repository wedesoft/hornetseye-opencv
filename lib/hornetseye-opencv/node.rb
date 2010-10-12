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
module Hornetseye

  class Node

    alias_method :orig_to_cvmat, :to_cvmat

    def to_cvmat
      if typecode < Composite
        # swap RGB!
        Hornetseye::MultiArray( typecode.element_type,
                                typecode.num_elements, *shape ).new( memory ).
          orig_to_cvmat
      elsif dimension == 2
        Hornetseye::MultiArray( typecode, 1, *shape ).new( memory ).
          orig_to_cvmat
      else
        orig_to_cvmat
      end
    end

  end

end
