create table if not exists public.owners (
    owner_id bigserial primary key,
    name varchar (255),
    birthday date,
    user_id bigint
);