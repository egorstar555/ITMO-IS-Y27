create table if not exists public.pets (
    pet_id bigserial primary key,
    name varchar (255),
    birthday date,
    color varchar (20),
    owner_id bigint,
    tail_length real
);