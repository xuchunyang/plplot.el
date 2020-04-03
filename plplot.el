;;; plplot.el --- Graph Plotting                     -*- lexical-binding: t; -*-

;; Copyright (C) 2020  Xu Chunyang

;; Author: Xu Chunyang
;; Homepage: https://github.com/xuchunyang/plplot.el
;; Package-Requires: ((emacs "25.1"))
;; Keywords: tools
;; Version: 0

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

;; An Emacs dynamic module for PLplot. It lets you plot, e.g.,
;;
;; (plplot #'sin (- pi) pi :outfile "sin.svg")

;;; Code:

(require 'plplot-module)
(require 'seq)                          ; `seq-into'

(cl-defun plplot (func x-min x-max
                       &key
                       (xlabel "x axis")
                       (ylabel "y axis")
                       (title "")
                       (samples 100)
                       (outfile (make-temp-file "plot" nil ".svg")))
  "Plot y=FUNC(x) from X-MIN to X-MAX."
  (setq x-min (float x-min)
        x-max (float x-max))
  (let* ((xs (cl-loop with step = (/ (- x-max x-min) samples)
                      for x from x-min to x-max by step
                      collect x))
         (ys (cl-loop for x in xs
                      collect (funcall func x))))
    ;; (xs ys xmin xmax ymin ymax xlabel ylabel title outfile)
    (plplot-module-plot
     (seq-into xs 'vector)
     (seq-into ys 'vector)
     (apply #'min xs)
     (apply #'max xs)
     (apply #'min ys)
     (apply #'max ys)
     xlabel
     ylabel
     title
     outfile)
    outfile))

(provide 'plplot)
;;; plplot.el ends here
