import logging
import logging.handlers

class Logger:
    def __init__(self, file_name='/root/YunMessenger.log'):
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.DEBUG)
        handler = logging.handlers.RotatingFileHandler(file_name, maxBytes=524288, backupCount=0)

        formatter = logging.Formatter()
        formatter = logging.Formatter("%(asctime)s %(levelname)s: %(message)s", '%Y-%m-%d %H:%M:%S')

        handler.setFormatter(formatter)

        self.logger.addHandler(handler)