# Qt pretty printers (from kdevelop-data package)
python
import sys, os.path
# Path to KDevelop Qt Pretty Printers directory
sys.path.insert(0, os.path.expanduser('/usr/share/kdevgdb/printers/'))

from qt import register_qt_printers
register_qt_printers (None)

from kde import register_kde_printers
register_kde_printers (None)
end
