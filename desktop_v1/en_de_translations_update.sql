drop trigger if exists en_de_translations_update;

create trigger en_de_translations_update
instead of update on en_de_translations
begin
    update texts set text = new.from_text
    where texts.text_id = (select from_text_id from translations where translations.trans_id = new.trans_id);    
    
    update texts set text = new.to_text
    where texts.text_id = (select to_text_id from translations where translations.trans_id = new.trans_id);
end;




