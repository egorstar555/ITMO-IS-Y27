create table if not exists public.pet_friends (
    pet_id bigint,
    friend_id bigint not null,
    primary key (pet_id, friend_id),
    constraint fk_pet_pet foreign key (pet_id) references public.pets (pet_id),
    constraint fk_friend_pet foreign key (friend_id) references public.pets (pet_id)
);