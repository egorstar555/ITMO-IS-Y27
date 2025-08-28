package com.starygin.petsms.service.mapper;

import com.starygin.petsms.entity.Pet;
import com.starygin.petsms.service.dto.PetDto;
import com.starygin.petsms.service.dto.PetSimpleDto;
import org.springframework.stereotype.Component;

import java.util.ArrayList;
import java.util.List;

@Component
public class PetMapper implements Mapper<Pet, PetDto> {
    @Override
    public Pet toEntity(PetDto dto) {
        Pet pet = new Pet();
        pet.setId(dto.id());
        pet.setName(dto.name());
        pet.setBirthday(dto.birthday());
        pet.setColor(dto.color());
        pet.setTailLength(dto.tailLength());
        pet.setOwner(dto.ownerId());

        return pet;
    }

    @Override
    public PetDto toDto(Pet entity) {
        List<PetSimpleDto> friends = new ArrayList<>();

        for (Pet pet : entity.getFriends()) {
            friends.add(new PetSimpleDto(pet.getId(), pet.getName(), pet.getBirthday(), pet.getColor(), pet.getTailLength(), pet.getOwner()));
        }

        return new PetDto(entity.getId(), entity.getName(), entity.getBirthday(), entity.getColor(), entity.getTailLength(), entity.getOwner(), friends);
    }
}
