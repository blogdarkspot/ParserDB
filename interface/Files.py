import os

class Files:

    def save_files(self, app, request):
        if 'file_delaf' in request.files:
            delaf = request.files['file_delaf']
            self.__save_file(app, delaf)
        if 'file_contracoes' in request.files:
            contracoes = request.files['file_contracoes']
            self.__save_file(app, contracoes)
        if 'file_gramatica' in request.files:
            gramatica = request.files['file_gramatica']
            self.__save_file(app, gramatica)

    def get_grammar_path(self, app):
        ret = ''
        for f in os.listdir(app.config['UPLOAD_FOLDER']):
            if f.endswith('.cnf'):
                ret = os.path.join(app.config['UPLOAD_FOLDER'], f)
        return ret

    
    def get_delaf_path(self, app):
        ret = ''
        for f in os.listdir(app.config['UPLOAD_FOLDER']):
            if f.endswith('.dic'):
                ret = os.path.join(app.config['UPLOAD_FOLDER'], f)
        return ret

    def get_contractions_path(self, app):
        ret = ''
        for f in os.listdir(app.config['UPLOAD_FOLDER']):
            if f.endswith('.cont'):
                ret = os.path.join(app.config['UPLOAD_FOLDER'], f)
        return ret 

    def get_terminals_path(self, app):
        ret = ''
        for f in os.listdir(app.config['UPLOAD_FOLDER']):
            if f.endswith('.term'):
                ret = os.path.join(app.config['UPLOAD_FOLDER'], f)
        return ret 

    def __save_file(self, app, file):
        if file.filename != '':
            filename = file.filename
            file.stream
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))

    
