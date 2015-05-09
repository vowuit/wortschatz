drop trigger if exists en_de_translations_insert;

create trigger en_de_translations_insert
instead of insert on en_de_translations
begin
    insert into texts (lang_id, text)
    select lang_id, new.from_text from languages where short = 'en';

    insert into texts (lang_id, text)
    select lang_id, new.to_text from languages where short = 'de';
    
    insert into translations (from_text_id, to_text_id)
    select last_insert_rowid()-1, last_insert_rowid();
end;




