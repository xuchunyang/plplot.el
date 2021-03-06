;;; plplot-tests.el --- Tests                        -*- lexical-binding: t; -*-

;; Copyright (C) 2020  Xu Chunyang

;; Author: Xu Chunyang

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <https://www.gnu.org/licenses/>.

;;; Commentary:

;; Tests for plplot.el

;;; Code:

(require 'plplot)
(require 'ert)

(ert-deftest plplot ()
  (should (plplot #'sin (- pi) pi)))

(ert-deftest plplot-bar-chart ()
  (should (plplot-bar-chart '(5 10 15 20 40))))

(provide 'plplot-tests)
;;; plplot-tests.el ends here
